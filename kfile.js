let project = new Project('Car');

project.cpp = true;

await project.addProject('Kore');

project.addFile('Sources/**');
project.setDebugDir('Deployment');

project.flatten();

resolve(project);
