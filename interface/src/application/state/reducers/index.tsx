import { combineReducers } from 'redux'

import { reducer } from '@electricui/core-redux-state'

const rootReducer = combineReducers({
  electricui: reducer,
})

export default rootReducer
