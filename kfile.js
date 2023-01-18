const project = new Project('Car');

// TODO: Windows CI builds, flatc compilation in CI
"https://github.com/google/flatbuffers/releases/latest/download/Windows.flatc.binary.zip"

const util = require('util');
const exec = util.promisify(require('child_process').exec);

{
	const { stdout, stderr } = await exec('flatc -c --scoped-enums --filename-suffix Generated --cpp-field-case-style lower --warnings-as-errors -o ./Sources/Game/Net/ ./protocol/*');
	if (stderr) {
		console.log('flatc error:', stdout, stderr);
	} else {
		console.log('flatc executed.', stdout, stderr);
	}
}
{
	const { stdout, stderr } = await exec('./fmt');
	if (stderr) {
		console.log('fmt error:', stdout, stderr);
	} else {
		console.log('fmt executed.', stdout, stderr);
	}
}
{
	const { stdout, stderr } = await exec('./installLibs.sh');
	if (stderr) {
		console.log('Lib installation error:', stdout, stderr);
	} else {
		console.log('Lib installation done.', stdout, stderr);
	}
}
{
	const { stdout, stderr } = await exec('./compileAssets.sh');
	if (stderr) {
		console.log('compileAssets error:', stdout, stderr);
	} else {
		console.log('compileAssets executed.', stdout, stderr);
	}
}

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
