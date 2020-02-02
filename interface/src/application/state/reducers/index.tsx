import { combineReducers } from 'redux'

import { reducer } from '@electricui/core-redux-state'

export const rootReducer = combineReducers({
  electricui: reducer,
})
