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
open Ava.Sync;

test("Should pass", t => {
  t.deepEqual(1, 1);
  t.pass();
});
```

```ml
open Ava.Async;

test("Should pass", t => {
  Js.Global.setTimeout(() => t.cb(), 1000)
  |> ignore
});
```

```ml
open Ava.Promise;

test("Should pass", t => {
  t.notThrows(Js.Promise.resolve("foo"));
});
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
  "scripts": {
    ...
    "test": "ava"
  },
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
    "excludeAfterRemap": true,
    "reporter": [
      "lcov",
      "text-summary"
    ]
  }
}
```
