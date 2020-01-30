module Styles = {
  open Css;
  let track = (_colors: ThemeContext.colors) =>
    style([
      height(`px(24)),
      width(`px(50)),
      cursor(`pointer),
      display(`flex),
      justifyContent(`spaceBetween),
      borderRadius(`px(30)),
      backgroundColor(`hex("000")),
      padding2(~v=`px(2), ~h=`px(3)),
      position(`relative),
    ]);

  let thumb = (colors: ThemeContext.colors, ~checked: bool) =>
    style([
      position(`absolute),
      top(`px(1)),
      bottom(`px(1)),
      left(`px(1)),
      width(`px(22)),
      borderRadius(`percent(50.0)),
      backgroundColor(`hex("fafafa")),
      transition(~duration=300, ~timingFunction=`easeInOut, "all"),
      transform(translateX(`px(checked ? 0 : 26))),
      active([
        boxShadow(
          Shadow.box(
            ~x=`zero,
            ~y=`zero,
            ~blur=`px(2),
            ~spread=`px(3),
            colors.primary,
          ),
        ),
      ]),
    ]);
};

[@react.component]
let make =
    (
      ~renderChecked: unit => React.element,
      ~renderUnchecked: unit => React.element,
      ~onChange: bool => unit,
      ~checked,
    ) => {
  let trackRef = React.useRef(Js.Nullable.null);
  let (theme, _) = React.useContext(ThemeContext.context);

  <div
    onClick={_ => onChange(!checked)}
    className={Styles.track(theme.colors)}
    ref={ReactDOMRe.Ref.domRef(trackRef)}>
    {renderUnchecked()}
    {renderChecked()}
    <div className={Styles.thumb(theme.colors, ~checked)} />
  </div>;
};