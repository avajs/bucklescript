# bs-ava

[![Build Status](https://travis-ci.com/ava/bucklescript.svg?branch=master)](https://travis-ci.com/ava/bucklescript)

**Experimental** [BuckleScript](https://github.com/bucklescript/bucklescript) bindings for [Ava](https://github.com/avajs/ava)

# Installation

```sh
npm install --save-dev ava@next @ava/bucklescript
```

Then add `@ava/bucklescript` to `bs-dev-dependencies` in your `bsconfig.json`:
```js
{
  ...
  "bs-dev-dependencies": ["@ava/bucklescript"]
}
```

# Getting started

```ml
open Sync;

test("Should pass", t => {
  t.deepEqual(1, 1);
  t.pass();
});
```

```ml
open Async;

test("Should pass", t => {
  Js.Global.setTimeout(() => t.end_(), 1000)
  |> ignore
});
```

```ml
open Promise;

test("Should pass", t => {
  t.notThrows(Js.Promise.resolve("foo"));
});
```

# Coverage

- Install `nyc`
```sh
npm i -D nyc
```

- Edit your `package.json`
```json
{
  "scripts": {
    ...
    "test": "nyc ava"
  },
  ...
  "nyc": {
    "reporter": [
      "lcov",
      "text-summary"
    ]
  }
}
```
