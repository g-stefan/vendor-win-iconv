//
// vendor-win-iconv.make
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xyo.hpp"

namespace Main {

	using namespace XYO;

	class Application :
		public virtual IMain {
			XYO_DISALLOW_COPY_ASSIGN_MOVE(Application);
		public:
			String includePath;
			String sourcePath;
			String buildPath;
			String utilPath;
			String binPath;
			String libPath;
			String testPath;
			String workspacePath;

			inline Application() {};

			void showUsage();
			void showLicense();

			int main(int cmdN, char *cmdS[]);
	};

	void Application::showUsage() {
		printf("Scope: build iconv.\n");
		printf("%s\n\n", XYO::Copyright::fullCopyright());

		printf("%s",
			"options:\n"
			"    --license           show license\n"
		);
		printf("\n");
	};

	void Application::showLicense() {
		printf("%s", XYO::License::content());
	};

	int Application::main(int cmdN, char *cmdS[]) {
		int i;
		String opt;
		size_t optIndex;
		String optValue;

		String softwareName = "win-iconv";
		String softwareVersion = "0.0.8";

		String applicationPath = Shell::getExecutablePath();
		String pathRepository = Shell::getEnv("XYO_PATH_REPOSITORY");
		if(pathRepository.length() == 0) {
			pathRepository = "../repository";
		};

		bool isRelease = true;
		bool doInstall = false;
		bool doRelease = false;
		bool cmdArchive = false;
		bool cmdExtract = false;

		for (i = 1; i < cmdN; ++i) {
			if (StringCore::beginWith(cmdS[i], "--")) {
				opt = &cmdS[i][2];
				optValue = "";
				if(String::indexOf(opt, "=", 0, optIndex)) {
					optValue = String::substring(opt, optIndex + 1);
					opt = String::substring(opt, 0, optIndex);
				};
				if (opt == "license") {
					showLicense();
					return 0;
				};
				continue;
			};
			if (strcmp(cmdS[i], "debug") == 0) {
				isRelease = false;
			};
			if (strcmp(cmdS[i], "install") == 0) {
				doInstall = true;
			};
			if (strcmp(cmdS[i], "install-release") == 0) {
				doRelease = true;
			};
			if (strcmp(cmdS[i], "archive") == 0) {
				cmdArchive = true;
			};
			if (strcmp(cmdS[i], "extract") == 0) {
				cmdExtract = true;
			};
			if (strcmp(cmdS[i], "test") == 0) {
				return 0;
			};
			if (strcmp(cmdS[i], "version") == 0) {
				return 0;
			};
		};

		if(!Shell::realPath(applicationPath + "/..", workspacePath)) {
			printf("ERROR: unable to get workspace real path\n");
			return 1;
		};

		// Archive

		Shell::chdir(workspacePath);

		String workspaceSource = softwareName + "-" + softwareVersion;

		if(cmdArchive) {
			Shell::chdir(workspacePath);

			// Make archive
			if(Shell::directoryExists(workspaceSource)) {
				String archive = "archive/";
				archive << workspaceSource << ".7z";
				if(Shell::fileExists(archive)) {
					if(!Shell::remove(archive)) {
						printf("Error: Unable to remove source archive\r\n");
						return 1;
					};
				};
				if(Shell::system(String("7zr a -mx9 -mmt4 -r- -sse -w. -y -t7z ") + archive + " " + workspaceSource)) {
					printf("Error: Unable to compress source\r\n");
					return 1;
				};
				return 0;
			};
			printf("Error: Source not found\r\n");
			return 1;
		};

		// Extract archive
		if(!Shell::directoryExists(workspaceSource)) {
			if(Shell::system(String("7zr x ") + workspacePath + "/archive/" + workspaceSource + ".7z")) {
				printf("Error: Unable to extract source archive: %s\r\n", workspaceSource.value());
				return 1;
			};
		};

		if(cmdExtract) {
			return 0;
		};

		// Build


		includePath = workspacePath;
		sourcePath = workspacePath;
		buildPath = workspacePath + "/build";
		binPath = workspacePath + "/bin";
		libPath = workspacePath + "/lib";

		int k;
		TDynamicArray<String> cppDefine;
		TDynamicArray<String> incPath;
		TDynamicArray<String> incFiles;
		TDynamicArray<String> cFiles;
		TDynamicArray<String> rcDefine;
		TDynamicArray<String> incPathRC;
		TDynamicArray<String> rcFiles;
		TDynamicArray<String> libDependencyPath;
		TDynamicArray<String> libDependency;

		String libVersion = "";

		cFiles.push(workspaceSource + "/win_iconv.c");

		incPath[0] = ".";
		incPath[1] = workspaceSource;

		incPathRC[0] = ".";
		incPathRC[1] = workspaceSource;

		if(!Compiler::makeCToLib(
				"win-iconv.static",
				binPath, libPath, buildPath,
				(isRelease ? CompilerOptions::Release : CompilerOptions::Debug) | CompilerOptions::CRTStatic | CompilerOptions::StaticLibrary,
				cppDefine, incPath, incFiles, cFiles,
				rcDefine, incPathRC, rcFiles,
				"",
				libDependencyPath, libDependency,
				libVersion,
				8,
				true
			)) {
			return 1;
		};

		cppDefine[0] = "MAKE_DLL";

		if(!Compiler::makeCToLib(
				"win-iconv",
				binPath, libPath, buildPath,
				(isRelease ? CompilerOptions::Release : CompilerOptions::Debug) | CompilerOptions::CRTDynamic | CompilerOptions::DynamicLibrary,
				cppDefine, incPath, incFiles, cFiles,
				rcDefine, incPathRC, rcFiles,
				workspaceSource + "/iconv.def",
				libDependencyPath, libDependency,
				libVersion,
				8,
				true
			)) {
			return 1;
		};

		cppDefine[0] = "MAKE_EXE";

		if(!Compiler::makeCToExe(
				"iconv",
				binPath, buildPath,
				(isRelease ? CompilerOptions::Release : CompilerOptions::Debug) | CompilerOptions::CRTDynamic,
				cppDefine, incPath, incFiles, cFiles,
				rcDefine, incPathRC, rcFiles,
				libDependencyPath, libDependency,
				8,
				true
			)) {
			return 1;
		};

		if(!Compiler::copyLib(libPath + "/win-iconv.static", libPath + "/iconv.static")) {
			return 1;
		};
		if(!Compiler::copyLib(libPath + "/win-iconv", libPath + "/iconv")) {
			return 1;
		};

		if(doRelease) {
			String pathRelease = Compiler::getPathReleaseVersion(softwareName, softwareVersion, isRelease);
			// Binary

			if(!Compiler::copyDllToFolder(binPath + "/win-iconv", pathRelease, "")) {
				return 1;
			};

			if(!Compiler::copyExeToFolder(binPath + "/iconv", pathRelease)) {
				return 1;
			};

			// Dev
			doInstall = true;
			pathRepository = pathRelease + "-dev";
		};

		if(doInstall) {
			if(!Compiler::copyLibToFolder(libPath + "/win-iconv.static", pathRepository + "/lib")) {
				return 1;
			};
			if(!Compiler::copyLibToFolder(libPath + "/win-iconv", pathRepository + "/lib")) {
				return 1;
			};
			if(!Compiler::copyLibToFolder(libPath + "/iconv.static", pathRepository + "/lib")) {
				return 1;
			};
			if(!Compiler::copyLibToFolder(libPath + "/iconv", pathRepository + "/lib")) {
				return 1;
			};
			if(!Compiler::copyDllToFolderWithLib(
					libPath + "/win-iconv", pathRepository + "/lib",
					binPath + "/win-iconv", pathRepository + "/bin",
					libVersion)) {
				return 1;
			};
			if(!Compiler::copyExeToFolder(binPath + "/iconv", pathRepository + "/bin")) {
				return 1;
			};
			if(!Shell::copyFile(workspaceSource + "/iconv.h", pathRepository + "/include/iconv.h")) {
				return 1;
			};
			if(!Shell::copyFile(workspaceSource + "/iconv.h", pathRepository + "/include/iconv/iconv.h")) {
				return 1;
			};
		};

		return 0;
	};

};

XYO_APPLICATION_MAIN_STD(Main::Application);
