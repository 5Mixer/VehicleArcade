let project = new Project('Car');

// TODO: Windows CI builds, flatc compilation in CI
"https://github.com/google/flatbuffers/releases/latest/download/Windows.flatc.binary.zip"

var exec = require('child_process').exec;
exec('flatc -c --scoped-enums --filename-suffix Generated --cpp-field-case-style lower --warnings-as-errors -o ./Sources/Game/Net/ ./protocol/*', function callback(error, stdout, stderr) {
	if (error) {
		console.log('flatc error:', error, stdout, stderr);
	} else {
		console.log('flatc executed.', stdout, stderr);
	}
});
exec('./fmt', function callback(error, stdout, stderr) {
	if (error) {
		console.log('fmt error:', error, stdout, stderr);
	} else {
		console.log('fmt executed.', stdout, stderr);
	}
});
exec('./installLibs.sh', function callback(error, stdout, stderr) {
	if (error) {
		console.log('Lib installation error:', error, stdout, stderr);
	} else {
		console.log('Lib installation done.', stdout, stderr);
	}
});

project.cpp = true;

await project.addProject('Kore');

project.addFile('Sources/**');
project.addFile('protocol/**');
project.setDebugDir('Deployment');
project.addCppFlag("-fdiagnostics-color=always")
// project.libs.push("enet")
// project.libs.push("flatbuffers")

project.flatten();

resolve(project);
