module.exports = {
  preset: 'ts-jest',
  testEnvironment: 'node',
  globals: {
    __DEV__: true,
    "ts-jest": {
      isolatedModules: false
    }
  },
  testRegex: '/test/.+(test|spec).[jt]sx?$',
  moduleFileExtensions: ['ts', 'tsx', 'js'],
  coveragePathIgnorePatterns: ['/node_modules/'],
  verbose: true,
  testRunner: 'jest-circus/runner',
}
