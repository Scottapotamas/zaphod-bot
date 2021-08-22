import { Store, applyMiddleware, createStore } from 'redux'

import { composeWithDevTools } from 'redux-devtools-extension'
import { rootReducer } from '../reducers/index'
import thunk from 'redux-thunk'

export const configureStore: () => Store = () => {
  try {
    // Redux Configuration
    const middleware = []

    // Thunk Middleware
    middleware.push(thunk)

    const composeEnhancers = composeWithDevTools({})

    const store = createStore(
      rootReducer,
      composeEnhancers(
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
  } catch (e) {
    console.error('Could not configure the store', e)
    throw e
  }
}
