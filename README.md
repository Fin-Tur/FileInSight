
# FileInSight

**FileInSight** is a file management and forensic analysis tool implemented in modern C++.  
It combines multiple features such as compression, encryption, file recovery, duplicate scanning, similarity detection, and more – all in one CLI application.

## 🔧 Features

- 🔍 **File Analytics**
  - Entropy analysis
  - Magic-Byte-Scanner to identify file types
  - Path analyzing
  - Age finder (last used > x days ago)

- 🧠 **Similarity and Duplicate Finder**
  - Hashing-based Duplicate Scanner
  - **TLSH** (Trend Micro Locality Sensitive Hashing) for fuzzy Similarity Detection
  - Comparison of Hashes, Entropy, Filenames etc.

- 📦 **Compression**
  - Huffman-Compression (implementated w/o libraries)
  - Zstandard (zstd) for professional Compression

- 🔐 **Encryption**
  - Simple XOR-based Encryption w/salt (Learning) experience
  - Safe AES encryption for real world use

- 🧰 **Recovery-Function**
  - File Recovery on .bin and .raw dumps using Magic Bytes (e.g. `.png`, `.zip`)

- 🖥 **Simple Usage**
  - Command-Line-Interface for all functions
  - Easily adjustable Settings (compression method and level, encryption method etc.)

- 🧪 **Testing**
- Catch2 unit testing framework integrated

---

## Preview and Usage

- **Commands**

<img width="929" height="313" alt="Screenshot 2025-07-25 112752" src="https://github.com/user-attachments/assets/f20a7370-42d9-4914-a8b6-13eafba8ba9f" />


- **Config**

<img width="392" height="128" alt="Screenshot 2025-07-25 112722" src="https://github.com/user-attachments/assets/13b4c7ea-3256-4fb2-a6b6-a54acb4d16e9" />


- **Dupes**

<img width="781" height="257" alt="Screenshot 2025-07-25 112944" src="https://github.com/user-attachments/assets/ccf3765c-9099-49e8-b34f-9f1b716f2ce2" />


---

## 🚀 Build-Instructions

### 🔨 Needs

- C++17-compatible Compiler (tested on GCC 13.1.0 and MinGW 11.0 w64)
- `CMake` ≥ 3.15

### ⚙️ Compiling

```bash
git clone https://github.com/Fin-Tur/fileinsight.git
cd fileinsight
mkdir build && cd build
cmake ..
make
