import { applyMiddleware, createStore, Store } from 'redux'
import thunk from 'redux-thunk'

import { rootReducer } from '../reducers'

const enhancer = applyMiddleware(thunk)

export const configureStore: () => Store = () => {
  const store = createStore(rootReducer, enhancer)

  return store
}
