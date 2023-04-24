const mymod = require('./burr-solver.js')
const FS = require("fs");

const xmlFile = FS.readFileSync("test.xml", "utf8");

mymod.onRuntimeInitialized = () => {
    console.log(mymod.solve(xmlFile))
}
