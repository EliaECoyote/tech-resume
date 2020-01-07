module Styles = {
  open Css;
  let resizerIndicator = (colors: ThemeContext.colors) =>
    style([
      position(`absolute),
      zIndex(-1),
      bottom(`px(-15)),
      right(`px(-15)),
      transform(`rotate(`deg(45.0))),
      height(`px(30)),
      width(`px(30)),
      backgroundColor(colors.accent),
    ]);
};

[@react.component]
let make =
  React.forwardRef(editorRef => {
    let (state, _) = React.useContext(ThemeContext.context);
    <div className={Styles.resizerIndicator(state.colors)} />;
  });