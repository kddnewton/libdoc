const { print } = require("./build/Release/binding.node");

const concat = (parts) => ({ type: "concat", parts });
const dedent = (contents) => ({ type: "align", contents, n: -1 });
const indent = (contents) => ({ type: "align", contents, n: 1 });
const group = (contents) => ({ type: "group", contents });
const hardline = { type: "line", hard: true };
const ifBreak = (breakContents, flatContents) => ({ type: "if-break", breakContents, flatContents });
const line = { type: "line" };
const literalline = { type: "line", hard: true, literal: true };
const softline = { type: "line", soft: true };

module.exports = {
  concat,
  dedent,
  indent,
  group,
  hardline,
  ifBreak,
  line,
  literalline,
  softline,
  print
};
