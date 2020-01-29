Open Scene - Parse scene. - Find how many frames - Find how many objects are in
the scene - Send frame information

Device Metadata for

- summary_file
- min frame,
- max frame
- available collections

- current frame
- current collection
- selected collections

UI displays progress based on current frame, drawing objects

UI can set selected collections.

##### Start button, disabled if "executing scene" to true or if current frame > max

- Set metadata "executing scene" to true
- Set current frame to min frame

- If current frame <= max frame "Run collection in frame", when done, repeat.

##### Stop button, disabled if "executing scene" to false

- Set metadata "executing scene" to false
- Pause light and movement queues
- Clear queue

For each frame For each collection Run collection in frame

##### Run collection in frame

- Set metadata "waiting on UI gate - start camera" to false
- Set metadata "waiting on UI gate - end camera" to false
- Set metadata "executing collection" to true
- Pause movement queue
- Pause light queue
- Clear queues

- Unpause movement queue,

- Wait until movement queue is >= the max queue depth
- Unpause light queue,

- Wait until light queue is >= the max queue depth
- "waiting on UI gate - start camera" to true - modal on UI side, if that and
  executing collection both true

- Wait until "waiting on UI gate - start camera" to false (the human should
  trigger the camera)
- send sync event

- we're now rendering the collection

- wait until both the movement queue is complete.
- raise a toaster if there are any lights that are still in the queue.
- clear both queues.

- "waiting on UI gate - end camera" to true - modal on UI side, if that and
  executing collection both true

- Wait until "waiting on UI gate - end camera" to false (the human should
  untrigger the camera)
- Set metadata "executing collection" to false
- increment current frame if next frame is <= max frame
