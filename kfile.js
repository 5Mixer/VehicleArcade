let project = new Project('Car');

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
