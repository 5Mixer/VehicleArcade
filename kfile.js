let project = new Project('Car');

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

project.cpp = true;

await project.addProject('Kore');

project.addFile('Sources/**');
project.addFile('protocol/**');
project.setDebugDir('Deployment');
project.addCppFlag("-fdiagnostics-color=always")
project.libs.push("enet")
project.libs.push("flatbuffers")

project.flatten();

resolve(project);
