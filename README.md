# unknown-msg

This is a very simple project that tests my understanding of how protobuf handles unknown message types. In particular, how can we serialize a message whose Descriptor is not available?

My solution: Define an "Unknown" message type which has no fields, and parse the unknown message into it. Then we can use protobuf's `Reflection` and `UnknownFieldSet` interfaces to inspect the deserialized message.

To try out this project, you'll need the default branch of [ign-msgs](https://bitbucket.org/ignitionrobotics/ign-msgs), which depends on [ign-cmake](https://bitbucket.org/ignitionrobotics/ign-cmake) and [protobuf](https://github.com/google/protobuf).

Configure, compile, then run the `write-ign-msg` executable. That will create two files in your current working directory.

After that, you can run `read-ign-msg`. By default, the executable will link to `ign-msgs`, which allows the executable to identify the message and parse it into its known structure.

However, if you comment out line 17, then the executable will not use any symbols from `ign-msgs` and the linker will therefore skip linking the executable to `ign-msgs`. As a side effect, the executable will not have access to the message's Descriptor. However, we can still deserialize the message data into the `Unknown` message type, and then use the `Reflection` interface to get the `UnknownFieldSet` and read out the data without knowing the original message type.
