# bs-ava

**Experimental** [BuckleScript](https://github.com/bucklescript/bucklescript) bindings for [Ava](https://github.com/avajs/ava)

# Installation

```sh
npm install --save-dev @godu/bs-ava
```

Then add `@godu/bs-ava` to `bs-dev-dependencies` in your `bsconfig.json`:
```js
{
  ...
  "bs-dev-dependencies": ["@godu/bs-ava"]
}
```

# Getting started

```ml
open Ava;

testBefore(_ => ());

testAfter(_ => ());

testBeforeEach(_ => ());

testAfterEach(_ => ());

test("Test#pass", t =>
  t.pass("pass")
);

testSerial("TestSerial#pass", t =>
  t.pass("serial")
);

testOnly("TestOnly#pass", t =>
  t.pass("only")
);

testSkip("TestSkip#pass", t =>
  t.pass("skip")
);

testTodo("TestTodo#pass");

testFailing("TestFailing#fail", t =>
  t.fail("fail")
);

test("Test#truthy", t =>
  t.truthy("truthy", true)
);

test("Test#falsy", t =>
  t.falsy("falsy", false)
);

test("Test#deepEqual", t =>
  t.deepEqual("deepEqual", ["foo"], ["foo"])
);
```

# Usage with bs-register

`bs-register` includes a require hook for `.re` files. This lets Ava run Reason files as tests.

- Install `@godu/bs-register`
```sh
npm i -D @godu/bs-register
```

- Edit your `package.json`
```json
{
  ...
  "ava": {
    "extensions": [
      "re"
    ],
    "compileEnhancements": false,
    "require": [
      "@godu/bs-register"
    ]
  }
}
```
