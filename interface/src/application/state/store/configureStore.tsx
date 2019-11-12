import { Store } from 'redux'

import { default as configureStoreDev } from './configureStore.dev'
import { default as configureStoreProd } from './configureStore.prod'

let configureStore: () => Store

if (process.env.NODE_ENV === 'production') {
  configureStore = configureStoreProd
} else {
  configureStore = configureStoreDev
}

export default configureStore
