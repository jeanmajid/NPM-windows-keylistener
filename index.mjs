import { createRequire } from "module";
const require = createRequire(import.meta.url);
const addon = require("./build/Release/keylistener.node");
export default addon;
