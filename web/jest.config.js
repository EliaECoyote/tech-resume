module.exports = {
  verbose: true,
  transform: {
    "^.+\\.[t|j]sx?$": "babel-jest"
  },
  transformIgnorePatterns: [
    "/node_modules/(?!@glennsl/bs-jest|bs-platform).+\\.js$"
  ]
}