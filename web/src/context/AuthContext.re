module InitialValue = {
  type fn('input, 'output) = 'input => 'output;

  external toReactCallback:
    fn('input, 'output) => React.callback('input, 'output) =
    "%identity";
};

type state = (
  UseAuth.status,
  React.callback(unit, Wonka_types.subscriptionT),
);

let context = React.createContext((UseAuth.Idle, () => ()));

module Provider = {
  let makeProps = (~value, ~children, ()) => {
    "value": value,
    "children": children,
  };
  let make = React.Context.provider(context);
};

[@react.component]
let make = (~children) => {
  let (authStatus, signOut) = UseAuth.hook();
  <Provider value=(authStatus, signOut)> children </Provider>;
};