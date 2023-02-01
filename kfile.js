const project = new Project('Car');

// TODO: Windows CI builds, flatc compilation in CI
"https://github.com/google/flatbuffers/releases/latest/download/Windows.flatc.binary.zip"

const promisify = require('util').promisify;
const fs = require('fs');
const exec = promisify(require('child_process').exec);

const flatcPath = "./flatc"
const flatcFlags = " -c --scoped-enums --filename-suffix Generated --cpp-field-case-style lower --warnings-as-errors"
const flatbuffersDir = "./protocol/"
const flatbuffersCache = "./protocol/cache/"
const flatbuffersDestination = './Sources/Game/Net/'

{
	await(promisify(fs.mkdir)(flatbuffersCache, { recursive: true }));

	const flatbufferFiles = fs.readdirSync(flatbuffersDir, { withFileTypes: true })
		.filter(item => !item.isDirectory())
		.map(item => item.name);

	for (const flatbufferFile of flatbufferFiles) {
		const { stdout, stderr } = await exec(`${flatcPath} ${flatcFlags} -o ${flatbuffersCache} ${flatbuffersDir}${flatbufferFile}`);
	}

	const flatbufferCacheFiles = fs.readdirSync(flatbuffersCache, { withFileTypes: true })
		.filter(item => !item.isDirectory())
		.map(item => item.name);


	for (const flatbufferCacheFile of flatbufferCacheFiles) {
		const source = `${flatbuffersCache}${flatbufferCacheFile}`;
		const destination = `${flatbuffersDestination}${flatbufferCacheFile}`
		const cacheContent = fs.readFileSync(source, { encoding: 'utf8', flag: 'r' });
		const existingContent = fs.readFileSync(destination, { encoding: 'utf8', flag: 'r' });
		if (cacheContent != existingContent) {
			fs.copyFile(source, destination, (err) => {
				if (err) throw err;
			});
		}
	}
}
{
	const { stdout, stderr } = await exec('./formatSource.sh');
	if (stderr) {
		console.log('formatSource.sh error:', stdout, stderr);
	} else {
		console.log('formatSource.sh executed.', stdout, stderr);
	}
}
{
	for (var i = 1; i < 70; i++) {
		exec(`./kravur/kravur ./Assets/fonts/SplineSansMono-Regular.ttf ${i} ./Deployment/fonts/font${i}.kravur`)
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
project.libs.push("fmt");

project.flatten();

resolve(project);
