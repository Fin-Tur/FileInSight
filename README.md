
# FileInSight

**FileInSight** is a file management and forensic analysis tool implemented in modern C++.  
It combines multiple features such as compression, encryption, file recovery, duplicate scanning, similarity detection, and more – all in one CLI application.
Library build is also possible, so extern use (as .dll or .so) is also possible.

## Features

-  **File Analytics**
  - Entropy analysis
  - Magic-Byte-Scanner to identify file types
  - Path analyzing
  - Age finder (last used > x days ago)
  - Chunk based file embedding generation and cosine similarity search to compare semantics of text files (**llama**)

-  **Similarity and Duplicate Finder**
  - Hashing-based Duplicate Scanner
  - **TLSH** (Trend Micro Locality Sensitive Hashing) for fuzzy Similarity Detection
  - Comparison of Hashes, Entropy, Filenames etc.

-  **Compression**
  - Huffman-Compression (implementated w/o libraries)
  - Zstandard (zstd) for professional Compression

-  **Encryption**
  - Simple XOR-based Encryption | Disclaimer: XOR encryption is included for educational purposes only. 
  - Safe AES encryption for real world use

-  **Recovery-Function**
  - File Recovery on .bin and .raw dumps using Magic Bytes (e.g. `.png`, `.zip`)

-  **Simple Usage**
  - Command-Line-Interface for all functions
  - Easily adjustable Settings (compression method and level, encryption method etc.)

-  **Testing**
  - Catch2 unit testing framework integrated

---


## Preview and Usage

- **Commands**

 
<img width="923" height="346" alt="Screenshot 2025-07-29 173854" src="https://github.com/user-attachments/assets/5e62cef2-6651-40c0-95d3-5d20a2378c66" />


- **Config**


<img width="576" height="133" alt="Screenshot 2025-07-29 173918" src="https://github.com/user-attachments/assets/57d3e0fa-7925-4f35-a11e-9e8bf010a7b4" />


- **Dupes**

 <img width="781" height="257" alt="Screenshot 2025-07-25 112944" src="https://github.com/user-attachments/assets/ccf3765c-9099-49e8-b34f-9f1b716f2ce2" />


---

### Needs

- C++17-compatible Compiler (tested on GCC 13.1.0 and MinGW 11.0 w64)
- `CMake` ≥ 3.15

### Compiling

```bash
git clone https://github.com/Fin-Tur/fileinsight.git
cd fileinsight
mkdir build && cd build
cmake ..
make
