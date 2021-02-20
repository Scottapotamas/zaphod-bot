import { React, useCallback } from 'react'
import { Toaster, Position } from '@blueprintjs/core'
import { Message } from '@electricui/core'
// import { useMessageSubscription } from '@electricui/components-core'

const ToastEmitter = Toaster.create({
  className: 'error-toaster',
  position: Position.TOP,
})

export function ToastConnector() {
  const toastFunction = useCallback((message: Message) => {
    if (message.messageID === 'error') {
      ToastEmitter.show(
        { message: message.payload, intent: 'warning' },
        message.payload,
      )
    }
  }, [])
  // useMessageSubscription(toastFunction)
  return null
}
