const scriptDir = "C:\\Users\\admin\\Documents\\sp_load"; // REPLACE WITH YOUR PATH

const dll = scriptDir + "\\build\\libsp_load.dll";

console.log("Loading:", dll);

const module = Module.load(dll);

console.log("Loaded:", module.name);
console.log("Base:", module.base);
