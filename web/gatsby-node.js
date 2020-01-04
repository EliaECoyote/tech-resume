const MonacoWebpackPlugin = require('monaco-editor-webpack-plugin')

exports.onCreateWebpackConfig = ({
  stage,
  rules,
  loaders,
  plugins,
  actions,
  getConfig,
}) => {
  const config = getConfig()
  // setting monaco-editor alias in order to ship only a subset of the features & languages
  // as described in this issue
  // https://github.com/Microsoft/monaco-editor-webpack-plugin/issues/43#issuecomment-436479402
  config.resolve.alias["monaco-editor"] = "monaco-editor/esm/vs/editor/editor.api"
  const monacoPlugin = new MonacoWebpackPlugin({ languages: ["markdown"] })
  config.plugins = config.plugins.concat(monacoPlugin)
}