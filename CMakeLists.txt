set(CMAKE_CXX_STANDARD 20)
#PROJECT(Archiver)
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wall")
add_executable(archiver
    archiver.cpp
        lib/exceptions.h lib/Huffman.cpp lib/Huffman.h lib/FileManaging/Reader.cpp lib/FileManaging/Reader.h lib/FileManaging/Writer.cpp lib/FileManaging/Writer.h lib/FileManaging/FileManagers.h lib/FileManaging/Hasher.h lib/FileManaging/Hasher.cpp lib/Structs/HuffmanBor.cpp lib/Structs/HuffmanBor.h lib/Structs/PriorityQueue.h)
add_catch(test_archive lib/exceptions.h lib/Huffman.cpp lib/Huffman.h lib/FileManaging/Reader.cpp lib/FileManaging/Reader.h lib/FileManaging/Writer.cpp lib/FileManaging/Writer.h lib/FileManaging/FileManagers.h lib/FileManaging/Hasher.h lib/FileManaging/Hasher.cpp lib/Structs/HuffmanBor.cpp lib/Structs/HuffmanBor.h lib/Structs/PriorityQueue.h test.cpp)