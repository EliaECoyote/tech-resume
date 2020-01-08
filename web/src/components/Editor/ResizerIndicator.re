module Styles = {
  open Css;
  let resizerSize = 30;
  let translateValue = resizerSize / 2 |> string_of_int;
  let container =
    style([alignSelf(`flexEnd), overflow(`hidden), zIndex(-1)]);
  let resizerIndicator = (colors: ThemeContext.colors) =>
    style([
      zIndex(-1),
      alignSelf(`flexEnd),
      unsafe(
        "transform",
        "translate("
        ++ translateValue
        ++ "px, "
        ++ translateValue
        ++ "px) rotate(45deg)",
      ),
      height(`px(resizerSize)),
      width(`px(resizerSize)),
      backgroundColor(colors.accent),
    ]);
};

[@react.component]
let make =
  React.forwardRef(editorRef => {
    let (state, _) = React.useContext(ThemeContext.context);
    <div className=Styles.container>
      <div className={Styles.resizerIndicator(state.colors)} />
    </div>;
  });