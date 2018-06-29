type passAssertion = unit => unit;
type failAssertion = unit => unit;
type truthyAssertion = bool => unit;
type falsyAssertion = bool => unit;
/* type deepEqualAssertion = 'a.('a, 'a) => unit; */

type assertions = {
  pass: passAssertion,
  fail: failAssertion,
  /* truthy: truthyAssertion,
     falsy: falsyAssertion,
     deepEqual: 'a .('a, 'a) => unit, */
};
type cbAssertions = {
  pass: passAssertion,
  /* fail: failAssertion,
     truthy: truthyAssertion,
     falsy: falsyAssertion,
     deepEqual: 'a .('a, 'a) => unit, */
  cb: (~error: Js.Exn.t=?, unit) => unit,
};

type executionContext = assertions;
type cbExecutionContext = cbAssertions;

type executionContextJS;
[@bs.send] external _pass : executionContextJS => unit = "pass";
[@bs.send] external _fail : executionContextJS => unit = "fail";
[@bs.send] external _end : executionContextJS => unit = "end";
let makeExecutionContext =
    (executionContextJS: executionContextJS)
    : executionContext => {
  pass: () => _pass(executionContextJS),
  fail: () => _fail(executionContextJS),
};
type cbExecutionContextJS;
[@bs.send] external _cb_pass : cbExecutionContextJS => unit = "pass";
[@bs.send]
external _cb_end : (cbExecutionContextJS, Js.Nullable.t(Js.Exn.t)) => unit =
  "end";
let makeCbExecutionContext =
    (cbExecutionContextJS: cbExecutionContextJS)
    : cbExecutionContext => {
  pass: () => _cb_pass(cbExecutionContextJS),
  cb: (~error: option(Js.Exn.t)=?, _) =>
    _cb_end(cbExecutionContextJS, Js.Nullable.fromOption(error)),
};

type implementationResult = unit;
type implementationResultJS = unit;
let makeImplementationResultJS: implementationResultJS => implementationResult =
  result => result;

type implementation = executionContext => implementationResult;
type cbImplementation = cbExecutionContext => implementationResult;

type implementationJS = executionContextJS => implementationResultJS;
type cbImplementationJS = cbExecutionContextJS => implementationResultJS;
let makeImplementation = (implementation, t) =>
  t |> makeExecutionContext |> implementation |> makeImplementationResultJS;
let makeCbImplementation = (cbImplementation, t) =>
  t |> makeCbExecutionContext |> cbImplementation |> makeImplementationResultJS;

type testInterface = (string, implementation) => unit;
type todoDeclaration = string => unit;
type onlyInterface = (string, implementation) => unit;
type skipInterface = (string, implementation) => unit;
type failingInterface = (string, implementation) => unit;
type serialInterface = (string, implementation) => unit;
type afterInterface = implementation => unit;
type beforeInterface = implementation => unit;
type cbInterface = (string, cbImplementation) => unit;
type alwaysInterface = cbImplementation => unit;
type hookCbInterface = cbImplementation => unit;
type hookCbSkipInterface = cbImplementation => unit;
type hookSkipInterface = implementation => unit;
type cbFailingInterface = (string, cbImplementation) => unit;
type cbOnlyInterface = (string, cbImplementation) => unit;
type cbSkipInterface = (string, cbImplementation) => unit;

module Sync = {
  [@bs.module "ava"]
  external _test : (string, executionContextJS => unit) => unit = "test";
  let test: testInterface =
    (message, implementation) =>
      _test(message, implementation |> makeImplementation);

  [@bs.module "ava"] [@bs.scope "test"]
  external _test_only : (string, executionContextJS => unit) => unit = "only";
  let test_only: onlyInterface =
    (message, implementation) =>
      _test_only(message, implementation |> makeImplementation);

  [@bs.module "ava"] [@bs.scope ("test", "failing")]
  external _test_failing_only : (string, executionContextJS => unit) => unit =
    "only";
  let test_failing_only: onlyInterface =
    (message, implementation) =>
      _test_failing_only(message, implementation |> makeImplementation);

  [@bs.module "ava"] [@bs.scope "test"]
  external _test_skip : (string, executionContextJS => unit) => unit = "skip";
  let test_skip: skipInterface =
    (message, implementation) =>
      _test_skip(message, implementation |> makeImplementation);

  [@bs.module "ava"] [@bs.scope ("test", "failing")]
  external _test_failing_skip : (string, executionContextJS => unit) => unit =
    "skip";
  let test_failing_skip: skipInterface =
    (message, implementation) =>
      _test_failing_skip(message, implementation |> makeImplementation);

  [@bs.module "ava"] [@bs.scope "test"]
  external _after : (executionContextJS => unit) => unit = "after";
  let after: afterInterface =
    implementation => _after(implementation |> makeImplementation);

  [@bs.module "ava"] [@bs.scope ("test", "after")]
  external _after_always : (executionContextJS => unit) => unit = "always";
  let after_always: afterInterface =
    implementation => _after_always(implementation |> makeImplementation);

  [@bs.module "ava"] [@bs.scope "test"]
  external _after_each : (executionContextJS => unit) => unit = "afterEach";
  let after_each: afterInterface =
    implementation => _after(implementation |> makeImplementation);

  [@bs.module "ava"] [@bs.scope ("test", "afterEach")]
  external _after_each_always : (executionContextJS => unit) => unit =
    "always";
  let after_each_always: afterInterface =
    implementation =>
      _after_each_always(implementation |> makeImplementation);

  [@bs.module "ava"] [@bs.scope "test"]
  external _before : (executionContextJS => unit) => unit = "before";
  let before: beforeInterface =
    implementation => _before(implementation |> makeImplementation);

  [@bs.module "ava"] [@bs.scope ("test", "before")]
  external _before_always : (executionContextJS => unit) => unit = "always";
  let before_always: beforeInterface =
    implementation => _before_always(implementation |> makeImplementation);

  [@bs.module "ava"] [@bs.scope "test"]
  external _before_each : (executionContextJS => unit) => unit = "beforeEach";
  let before_each: beforeInterface =
    implementation => _before_each(implementation |> makeImplementation);

  [@bs.module "ava"] [@bs.scope ("test", "beforeEach")]
  external _before_each_always : (executionContextJS => unit) => unit =
    "always";
  let before_each_always: beforeInterface =
    implementation =>
      _before_each_always(implementation |> makeImplementation);

  [@bs.module "ava"] [@bs.scope "test"]
  external _todo : string => unit = "todo";
  let todo: todoDeclaration = message => _todo(message);

  module Serial = {
    [@bs.module "ava"] [@bs.scope "test"]
    external _test : (string, executionContextJS => unit) => unit = "serial";
    let test: testInterface =
      (message, implementation) =>
        _test(message, implementation |> makeImplementation);

    [@bs.module "ava"] [@bs.scope ("test", "serial")]
    external _test_only : (string, executionContextJS => unit) => unit =
      "only";
    let test_only: onlyInterface =
      (message, implementation) =>
        _test_only(message, implementation |> makeImplementation);

    /* [@bs.module "ava"] [@bs.scope ("test", "serial", "failing")]
       external _test_failing_only : (string, executionContextJS => unit) => unit =
         "only";
       let test_failing_only: onlyInterface =
         (message, implementation) =>
           _test_failing_only(message, implementation |> makeImplementation); */

    [@bs.module "ava"] [@bs.scope ("test", "serial")]
    external _test_skip : (string, executionContextJS => unit) => unit =
      "skip";
    let test_skip: skipInterface =
      (message, implementation) =>
        _test_skip(message, implementation |> makeImplementation);

    /* [@bs.module "ava"] [@bs.scope ("test", "serial", "failing")]
       external _test_failing_skip : (string, executionContextJS => unit) => unit =
         "skip";
       let test_failing_skip: skipInterface =
         (message, implementation) =>
           _test_failing_skip(message, implementation |> makeImplementation); */

    [@bs.module "ava"] [@bs.scope ("test", "serial")]
    external _after : (executionContextJS => unit) => unit = "after";
    let after: afterInterface =
      implementation => _after(implementation |> makeImplementation);

    [@bs.module "ava"] [@bs.scope ("test", "serial", "after")]
    external _after_always : (executionContextJS => unit) => unit = "always";
    let after_always: afterInterface =
      implementation => _after_always(implementation |> makeImplementation);

    [@bs.module "ava"] [@bs.scope ("test", "serial")]
    external _after_each : (executionContextJS => unit) => unit = "afterEach";
    let after_each: afterInterface =
      implementation => _after(implementation |> makeImplementation);

    [@bs.module "ava"] [@bs.scope ("test", "serial", "afterEach")]
    external _after_each_always : (executionContextJS => unit) => unit =
      "always";
    let after_each_always: afterInterface =
      implementation =>
        _after_each_always(implementation |> makeImplementation);

    [@bs.module "ava"] [@bs.scope ("test", "serial")]
    external _before : (executionContextJS => unit) => unit = "before";
    let before: beforeInterface =
      implementation => _before(implementation |> makeImplementation);

    [@bs.module "ava"] [@bs.scope ("test", "serial", "before")]
    external _before_always : (executionContextJS => unit) => unit = "always";
    let before_always: beforeInterface =
      implementation => _before_always(implementation |> makeImplementation);

    [@bs.module "ava"] [@bs.scope ("test", "serial")]
    external _before_each : (executionContextJS => unit) => unit =
      "beforeEach";
    let before_each: beforeInterface =
      implementation => _before_each(implementation |> makeImplementation);

    [@bs.module "ava"] [@bs.scope ("test", "serial")]
    external _todo : string => unit = "todo";
    let todo: todoDeclaration = message => _todo(message);
  };
};

module Async = {
  [@bs.module "ava"] [@bs.scope "test"]
  external _test : (string, cbExecutionContextJS => unit) => unit = "cb";
  let test: cbInterface =
    (message, implementation) =>
      _test(message, implementation |> makeCbImplementation);

  [@bs.module "ava"] [@bs.scope ("test", "cb")]
  external _test_only : (string, cbExecutionContextJS => unit) => unit =
    "only";
  let test_only: cbOnlyInterface =
    (message, implementation) =>
      _test_only(message, implementation |> makeCbImplementation);

  [@bs.module "ava"] [@bs.scope ("test", "cb", "failing")]
  external _test_failing_only : (string, cbExecutionContextJS => unit) => unit =
    "only";
  let test_failing_only: cbOnlyInterface =
    (message, implementation) =>
      _test_failing_only(message, implementation |> makeCbImplementation);

  [@bs.module "ava"] [@bs.scope ("test", "cb")]
  external _test_skip : (string, cbExecutionContextJS => unit) => unit =
    "skip";
  let test_skip: cbSkipInterface =
    (message, implementation) =>
      _test_skip(message, implementation |> makeCbImplementation);

  [@bs.module "ava"] [@bs.scope ("test", "cb", "failing")]
  external _test_failing_skip : (string, cbExecutionContextJS => unit) => unit =
    "skip";
  let test_failing_skip: cbSkipInterface =
    (message, implementation) =>
      _test_failing_skip(message, implementation |> makeCbImplementation);

  [@bs.module "ava"] [@bs.scope ("test", "after")]
  external _after : (cbExecutionContextJS => unit) => unit = "cb";
  let after: hookCbInterface =
    implementation => _after(implementation |> makeCbImplementation);

  [@bs.module "ava"] [@bs.scope ("test", "after", "always")]
  external _after_always : (cbExecutionContextJS => unit) => unit = "cb";
  let after_always: hookCbInterface =
    implementation => _after_always(implementation |> makeCbImplementation);

  [@bs.module "ava"] [@bs.scope ("test", "afterEach")]
  external _after_each : (cbExecutionContextJS => unit) => unit = "cb";
  let after_each: hookCbInterface =
    implementation => _after(implementation |> makeCbImplementation);

  [@bs.module "ava"] [@bs.scope ("test", "afterEach", "always")]
  external _after_each_always : (cbExecutionContextJS => unit) => unit = "cb";
  let after_each_always: hookCbInterface =
    implementation =>
      _after_each_always(implementation |> makeCbImplementation);

  [@bs.module "ava"] [@bs.scope ("test", "before")]
  external _before : (cbExecutionContextJS => unit) => unit = "cb";
  let before: hookCbInterface =
    implementation => _before(implementation |> makeCbImplementation);

  [@bs.module "ava"] [@bs.scope ("test", "before", "always")]
  external _before_always : (cbExecutionContextJS => unit) => unit = "cb";
  let before_always: hookCbInterface =
    implementation => _before_always(implementation |> makeCbImplementation);

  [@bs.module "ava"] [@bs.scope ("test", "beforeEach")]
  external _before_each : (cbExecutionContextJS => unit) => unit = "cb";
  let before_each: hookCbInterface =
    implementation => _before_each(implementation |> makeCbImplementation);

  [@bs.module "ava"] [@bs.scope ("test", "beforeEach", "always")]
  external _before_each_always : (cbExecutionContextJS => unit) => unit = "cb";
  let before_each_always: hookCbInterface =
    implementation =>
      _before_each_always(implementation |> makeCbImplementation);

  [@bs.module "ava"] [@bs.scope "test"]
  external _todo : string => unit = "todo";
  let todo: todoDeclaration = message => _todo(message);

  module Serial = {
    [@bs.module "ava"] [@bs.scope ("test", "serial")]
    external _test : (string, cbExecutionContextJS => unit) => unit = "cb";
    let test: cbInterface =
      (message, implementation) =>
        _test(message, implementation |> makeCbImplementation);

    /* [@bs.module "ava"] [@bs.scope ("test", "serial", "only")]
       external _test_only : (string, cbExecutionContextJS => unit) => unit =
         "cb";
       let test_only: cbOnlyInterface =
         (message, implementation) =>
           _test_only(message, implementation |> makeCbImplementation); */

    /* [@bs.module "ava"] [@bs.scope ("test", "serial", "failing", "only")]
       external _test_failing_only :
         (string, cbExecutionContextJS => unit) => unit =
         "cb";
       let test_failing_only: cbOnlyInterface =
         (message, implementation) =>
           _test_failing_only(message, implementation |> makeCbImplementation); */

    /* [@bs.module "ava"] [@bs.scope ("test", "serial", "skip")]
       external _test_skip : (string, cbExecutionContextJS => unit) => unit =
         "cb";
       let test_skip: cbSkipInterface =
         (message, implementation) =>
           _test_skip(message, implementation |> makeCbImplementation); */

    /* [@bs.module "ava"] [@bs.scope ("test", "serial", "failing", "skip")]
       external _test_failing_skip :
         (string, cbExecutionContextJS => unit) => unit =
         "cb";
       let test_failing_skip: cbSkipInterface =
         (message, implementation) =>
           _test_failing_skip(message, implementation |> makeCbImplementation); */

    [@bs.module "ava"] [@bs.scope ("test", "serial", "after")]
    external _after : (cbExecutionContextJS => unit) => unit = "cb";
    let after: hookCbInterface =
      implementation => _after(implementation |> makeCbImplementation);

    [@bs.module "ava"] [@bs.scope ("test", "serial", "after", "always")]
    external _after_always : (cbExecutionContextJS => unit) => unit = "cb";
    let after_always: hookCbInterface =
      implementation => _after_always(implementation |> makeCbImplementation);

    [@bs.module "ava"] [@bs.scope ("test", "serial", "afterEach")]
    external _after_each : (cbExecutionContextJS => unit) => unit = "cb";
    let after_each: hookCbInterface =
      implementation => _after(implementation |> makeCbImplementation);

    [@bs.module "ava"] [@bs.scope ("test", "serial", "afterEach", "always")]
    external _after_each_always : (cbExecutionContextJS => unit) => unit =
      "cb";
    let after_each_always: hookCbInterface =
      implementation =>
        _after_each_always(implementation |> makeCbImplementation);

    [@bs.module "ava"] [@bs.scope ("test", "serial", "before")]
    external _before : (cbExecutionContextJS => unit) => unit = "cb";
    let before: hookCbInterface =
      implementation => _before(implementation |> makeCbImplementation);

    [@bs.module "ava"] [@bs.scope ("test", "serial", "before", "always")]
    external _before_always : (cbExecutionContextJS => unit) => unit = "cb";
    let before_always: hookCbInterface =
      implementation =>
        _before_always(implementation |> makeCbImplementation);

    [@bs.module "ava"] [@bs.scope ("test", "serial", "beforeEach")]
    external _before_each : (cbExecutionContextJS => unit) => unit = "cb";
    let before_each: hookCbInterface =
      implementation => _before_each(implementation |> makeCbImplementation);

    [@bs.module "ava"] [@bs.scope ("test", "serial")]
    external _todo : string => unit = "todo";
    let todo: todoDeclaration = message => _todo(message);
  };
};
