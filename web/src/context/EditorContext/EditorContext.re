type state = ServiceMonacoEditor.monacoEditorServiceT;

let context = React.createContext(ServiceMonacoEditor.make());

module Provider = {
  let makeProps = (~value, ~children, ()) => {
    "value": value,
    "children": children,
  };
  let make = React.Context.provider(context);
};

[@react.component]
let make = (~children) => {
  let (themeState, _) = React.useContext(ThemeContext.context);
  let monacoEditorService = UseLazyRef.hook(ServiceMonacoEditor.make);

  // reacts to *theme* change by updating Monaco editor theme
  React.useEffect1(
    () => {
      Monaco.setTheme(. themeState.theme);
      None;
    },
    [|themeState.theme|],
  );

  <Provider value=monacoEditorService> children </Provider>;
};