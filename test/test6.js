import Module from '../wasm/burr-tools.js'
import {readFileSync} from "fs"
// const FS = require("fs");
const xmlFile = readFileSync("test.xml", "utf8");
var mymod = null

Module().then(myModule => {  
    mymod = myModule
    console.log(mymod.solve(xmlFile))
  });
