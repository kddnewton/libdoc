const { concat, group, line, print } = require("../index");

test("basic", () => {
  const node = group(concat(["Hello", line, "world!"]));
  const actual = print(node, { lineLength: 80, tabWidth: 2 });

  expect(actual).toEqual("Hello world!");
});

test("broken", () => {
  const node = group(concat(["Hello", line, "world!"]));
  const actual = print(node, { lineLength: 4, tabWidth: 2 });

  expect(actual).toEqual("Hello\nworld!");
});
