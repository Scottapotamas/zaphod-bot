module.exports = {
  preset: "ts-jest",
  testEnvironment: "node",
  globals: {
    __DEV__: true,
  },
  testRegex: "/test/.+(test|spec).[jt]sx?$",
  moduleFileExtensions: ["ts", "tsx", "js"],
  coveragePathIgnorePatterns: ["/node_modules/"],
  verbose: true,
  testRunner: "jest-circus/runner",
  transformIgnorePatterns: [
    "node_modules/(?!(@electricui)/)", // Allow Electric UI modules to be transformed
  ],
  transform: {
    "\\.eui.js$": "@electricui/jest-transform", // Transform Electric UI modules
    "^.+\\.tsx?$": ["ts-jest", { isolatedModules: false }],
  },
};
