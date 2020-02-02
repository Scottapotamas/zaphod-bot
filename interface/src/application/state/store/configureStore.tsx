import { Store } from 'redux'

import { configureStore as configureStoreProd } from './configureStore.prod'
import { configureStore as configureStoreDev } from './configureStore.dev'

let configureStoreWithEnv: () => Store

if (process.env.NODE_ENV === 'production') {
  configureStoreWithEnv = configureStoreProd
} else {
  configureStoreWithEnv = configureStoreDev
}

export const configureStore = configureStoreWithEnv
