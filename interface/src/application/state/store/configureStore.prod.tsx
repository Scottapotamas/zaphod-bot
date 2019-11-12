import { applyMiddleware, createStore, Store } from 'redux'
import thunk from 'redux-thunk'

import rootReducer from '../reducers'

const enhancer = applyMiddleware(thunk)

const configureStore: () => Store = () => {
  const store = createStore(rootReducer, enhancer)

  return store
}

export default configureStore
