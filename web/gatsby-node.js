const MonacoWebpackPlugin = require('monaco-editor-webpack-plugin');

exports.onCreateWebpackConfig = ({
  stage,
  rules,
  loaders,
  plugins,
  actions,
  getConfig,
}) => {
  const config = getConfig();
  config.plugins = config.plugins.concat(new MonacoWebpackPlugin()) 
}