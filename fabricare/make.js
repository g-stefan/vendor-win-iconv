// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

Fabricare.include("vendor");

messageAction("make");

if (Shell.fileExists("temp/build.done.flag")) {
	return;
};

if (!Shell.directoryExists("source")) {
	exitIf(Shell.system("7z x -aoa archive/" + Project.vendor + ".7z"));
	Shell.rename(Project.vendor, "source");
};

Shell.mkdirRecursivelyIfNotExists("output");
Shell.mkdirRecursivelyIfNotExists("output/bin");
Shell.mkdirRecursivelyIfNotExists("output/include");
Shell.mkdirRecursivelyIfNotExists("output/lib");
Shell.mkdirRecursivelyIfNotExists("temp");

Shell.mkdirRecursivelyIfNotExists("output/include");
Shell.copyFile("source/iconv.h","output/include/iconv.h");
Shell.mkdirRecursivelyIfNotExists("output/include/iconv");
Shell.copyFile("source/iconv.h","output/include/iconv/iconv.h");

global.xyoCCExtra = function() {
	arguments.push(

	    "--inc=output/include",
	    "--use-lib-path=output/lib",
	    "--rc-inc=output/include",

	    "--inc=" + pathRepository + "/include",
	    "--use-lib-path=" + pathRepository + "/lib",
	    "--rc-inc=" + pathRepository + "/include"

	);
	return arguments;
};


var compileProject = {
	"project" : "win-iconv",
	"includePath" : [
		"output/include",
		"source",
	],
	"cSource" : [
		"source/win_iconv.c"
	]
};

Shell.filePutContents("temp/" + compileProject.project + ".compile.json", JSON.encodeWithIndentation(compileProject));
exitIf(xyoCC.apply(null, xyoCCExtra("@temp/" + compileProject.project + ".compile.json", "--lib", "--output-lib-path=output/lib", "--crt-static")));
Shell.copyFile("output/lib/win-iconv.static.lib","output/lib/iconv.static.lib");

var compileProject = {
	"project" : "win-iconv",
	"defines" : [
		"MAKE_DLL",
	],
	"includePath" : [
		"output/include",
		"source",
	],
	"cSource" : [
		"source/win_iconv.c"
	],
	"linkerDefinitionsFile" : "source/iconv.def"
};

Shell.filePutContents("temp/" + compileProject.project + ".compile.json", JSON.encodeWithIndentation(compileProject));
exitIf(xyoCC.apply(null, xyoCCExtra("@temp/" + compileProject.project + ".compile.json", "--dll", "--output-bin-path=output/bin", "--output-lib-path=output/lib")));
Shell.copyFile("output/lib/win-iconv.lib","output/lib/iconv.lib");

var compileProject = {
	"project" : "win-iconv",
	"defines" : [
		"MAKE_EXE",
	],
	"includePath" : [
		"output/include",
		"source",
	],
	"cSource" : [
		"source/win_iconv.c"
	],
	"library" : [
		"win-iconv.static"
	]
};

Shell.filePutContents("temp/" + compileProject.project + ".compile.json", JSON.encodeWithIndentation(compileProject));
exitIf(xyoCC.apply(null, xyoCCExtra("@temp/" + compileProject.project + ".compile.json", "--exe", "--output-bin-path=output/bin", "--crt-static")));

