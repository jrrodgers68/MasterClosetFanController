# New Architecture for MQTT based applications

## Main Classes

Publisher - provides clean interface for app to publish values/data

Serializer - understands how messages of given types are serialized (i.e. the payloads)

PayloadFormatter - an interface for the specific details about serializing a message, such as how a temperature payload is read or written to JSON

Concrete implementations of PayloadFormatter interface - such as TemperaturePayloadFormatter, LogPayloadFormatter

TopicMgr - singleton that manages mapping of "names" to topic strings

JsonMQTTMsgMgr - singleton that manages messages sent to MQTT.  So will handle resubmitting any QOS1 messages that don't get ACK'd.  Deals with buffers as just an array of chars, no understanding of the format itself.  

BufferMgr - provides buffer objects to the rest of the system, esp for message payloads.  


## Design Considerations

One of the consequences of this approach is that a given message type will always be encoded the same way - e.g. a temperature message will always be encoded as json and not some other format is another context.

To expand the system to new message types, you would first provide a new concrete implementation of the PayloadFormatter interface.  Then the Serializer would need to know about it and when to use it.  Finally, a simple calling interface would be provided on the Publisher so the application can simply send the new message type via a single call on the Publisher.

The question arises as to wether the Serializer is really providing much value.  The publisher has to call specific methods on it as it basically understands what type of message it wants to create - the methods on Publisher are very specific.  I guess the main value would be that you could swap in a different serializer and end up formatting messages in a different format.
