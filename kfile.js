let project = new Project('Car');

project.cpp = true;

await project.addProject('Kore');

project.addFile('Sources/**');
project.setDebugDir('Deployment');
project.addCppFlag("-fdiagnostics-color=always")
project.libs.push("enet")

project.flatten();

resolve(project);
