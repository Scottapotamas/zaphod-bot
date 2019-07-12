import { applyMiddleware, createStore, Store } from 'redux'
import { composeWithDevTools } from 'redux-devtools-extension'
import thunk from 'redux-thunk'

// import { createLogger } from 'redux-logger'
import rootReducer from '../reducers/index'

const configureStore: () => Store = () => {
  // Redux Configuration
  const middleware = []

  // Thunk Middleware
  middleware.push(thunk)

  const store = createStore(
    rootReducer,
    composeWithDevTools(
      applyMiddleware(...middleware),
      // other store enhancers if any
    ),
  )

  if (module.hot) {
    module.hot.accept(
      '../reducers/index',
      () => store.replaceReducer(require('../reducers/index')), // eslint-disable-line global-require
    )
  }

  return store
}
export default configureStore
