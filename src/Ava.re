type passAssertion = unit => unit;
type failAssertion = unit => unit;
type truthyAssertion = bool => unit;
type falsyAssertion = bool => unit;
/* type deepEqualAssertion = 'a.('a, 'a) => unit; */

type assertions = {
  pass: passAssertion,
  fail: failAssertion,
  truthy: truthyAssertion,
  falsy: falsyAssertion,
  deepEqual: 'a.('a, 'a) => unit,
};

type executionContext = assertions;

type implementationResult = unit;

type implementation = executionContext => implementationResult;

type todoDeclaration = (string) => unit;
type onlyInterface = (string, implementation) => unit;
type skipInterface = (string, implementation) => unit;
type failingInterface = {
  run: (string, implementation) => unit,
  skip: skipInterface,
  only: onlyInterface,
}
type serialInterface = {
  run: (string, implementation) => unit,
  failing: unit => failingInterface,
  only: onlyInterface,
  skip: skipInterface,
  todo: todoDeclaration,
};
type testInterface = {
  run: (string, implementation) => unit,
  todo: todoDeclaration,
  failing: unit => failingInterface,
  serial: unit => serialInterface,
  skip: skipInterface,
  only: onlyInterface,
};

type implementationResultJS = unit;

type executionContextJS = {
  pass: unit => unit,
  fail: unit => unit,
  truthy: (bool) => unit,
  falsy: (bool) => unit,
  deepEqual: 'a.('a, 'a) => unit,
};

type implementationJS = executionContextJS => implementationResultJS;

type failingInterfaceJS = {
  call: (failingInterfaceJS, string, testInterface) => unit,
  skip: failingInterfaceJS,
  only: (failingInterfaceJS, string, testInterface) => unit
};
type serialInterfaceJS = {
  call: (serialInterfaceJS, string, testInterface) => unit,
  only: (serialInterfaceJS, string, testInterface) => unit,
  skip: (serialInterfaceJS, string, testInterface) => unit,
  todo: (serialInterfaceJS, string) => unit,
};
type testInterfaceJS = {
  call: (testInterfaceJS, string, testInterface) => unit,
  todo: (testInterfaceJS, string) => unit,
  failing: testInterfaceJS,
  serial: testInterfaceJS,
  skip: (testInterfaceJS, string, testInterface) => unit,
  only: (testInterfaceJS, string, testInterface) => unit,
};

[@bs.send] external _passAssertion : (executionContextJS) => unit = "pass";
[@bs.send] external _failAssertion : (executionContextJS) => unit = "fail";
[@bs.send] external _truthyAssertion : (executionContextJS, bool) => unit = "truthy";
[@bs.send] external _falsyAssertion : (executionContextJS, bool) => unit = "falsy";
[@bs.send] external _deepEqualTest : (executionContextJS, 'a, 'a) => unit = "deepEqual";

let executionContextFactory = (t: executionContextJS): executionContext => {
  pass: () => {
    _passAssertion(t);
  },
  fail: () => {
    _failAssertion(t);
  },
  truthy: (actual) => {
    _truthyAssertion(t, actual);
  },
  falsy: (actual) => {
    _falsyAssertion(t, actual);
  },
  deepEqual: (expected, actual) => {
    _deepEqualTest(t, actual, expected);
  },
};

[@bs.send] external _callFailingTest : (failingInterfaceJS, failingInterfaceJS, string, implementationJS) => unit = "call";
[@bs.send] external _callSkipFailingTest : (failingInterfaceJS, string, implementationJS) => unit = "skip";
[@bs.send] external _callOnlyFailingTest : (failingInterfaceJS, string, implementationJS) => unit = "only";
let failingInterfaceFactory = (test: failingInterfaceJS) : failingInterface => {
  run: (message, implementation) => {
    _callFailingTest(test, test, message, tJS => {
      implementation(executionContextFactory(tJS));
    });
  },
  skip: (message, implementation) => {
    _callSkipFailingTest(test, message, tJS => {
      implementation(executionContextFactory(tJS));
    });
  },
  only: (message, implementation) => {
    _callOnlyFailingTest(test, message, tJS => {
      implementation(executionContextFactory(tJS));
    });
  },
};

[@bs.send] external _callSerialTest : (serialInterfaceJS, serialInterfaceJS, string, implementationJS) => unit = "call";
[@bs.send] external _callSkipSerialTest : (serialInterfaceJS, string, implementationJS) => unit = "skip";
[@bs.send] external _callOnlySerialTest : (serialInterfaceJS, string, implementationJS) => unit = "only";
[@bs.send] external _callTodoSerialTest : (serialInterfaceJS, string) => unit = "todo";
[@bs.get] external _getFailingSerialTest : serialInterfaceJS => failingInterfaceJS = "failing";
let serialInterfaceFactory = (test: serialInterfaceJS) : serialInterface => {
  run: (message, implementation) => {
    _callSerialTest(test, test, message, tJS => {
      implementation(executionContextFactory(tJS));
    });
  },
  skip: (message, implementation) => {
    _callSkipSerialTest(test, message, tJS => {
      implementation(executionContextFactory(tJS));
    });
  },
  only: (message, implementation) => {
    _callOnlySerialTest(test, message, tJS => {
      implementation(executionContextFactory(tJS));
    });
  },
  failing: () => {
    test |> _getFailingSerialTest |> failingInterfaceFactory;
  },
  todo: (message) => {
    _callTodoSerialTest(test, message);
  },
};

[@bs.send] external _callTest : (testInterfaceJS, testInterfaceJS, string, implementationJS) => unit = "call";
[@bs.send] external _callTodoTest : (testInterfaceJS, string) => unit = "todo";
[@bs.get] external _getFailingTest : testInterfaceJS => failingInterfaceJS = "failing";
[@bs.get] external _getSerialTest : testInterfaceJS => serialInterfaceJS = "serial";
[@bs.send] external _callSkipTest : (testInterfaceJS, string, implementationJS) => unit = "skip";
[@bs.send] external _callOnlyTest : (testInterfaceJS, string, implementationJS) => unit = "only";
let testInterfaceFactory = (test: testInterfaceJS): testInterface =>  {
  run: (message, implementation) => {
    _callTest(test, test, message, tJS => {
      implementation(executionContextFactory(tJS));
    });
  },
  todo: (message) => {
    _callTodoTest(test, message);
  },
  failing: () => {
    test |> _getFailingTest |> failingInterfaceFactory;
  },
  serial: () => {
    test |> _getSerialTest |> serialInterfaceFactory;
  },
  skip: (message, implementation) => {
    _callSkipTest(test, message, tJS => {
      implementation(executionContextFactory(tJS));
    });
  },
  only: (message, implementation) => {
    _callOnlyTest(test, message, tJS => {
      implementation(executionContextFactory(tJS));
    });
  },
};

[@bs.module "ava"]
external testJS : testInterfaceJS = "test";
let test = testInterfaceFactory(testJS);
