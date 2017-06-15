# Encrypher Application
## Build
Build with Visual Studio 2017 with `Desktop development with C++` installed. For maximum of speed, build with `Ox` or `O2` build argument.
## Usage
```
Encrypher FileName [Password]
```
You can also drag and drop the file you want on the application to encrypt the file without password. **An internet connection is required to encrypt a file.**
### To Do List
- [x] Multi-Threading encryption
- [ ] Encrypt multi files and folders
- [ ] Custom encrypt block size(Default is 100MB) [See Here](https://github.com/HirbodBehnam/Encrypher/blob/master/Encrypher/Encrypher.cpp#L189)
#### Idea
The idea came from the game [Hacknet](http://www.hacknet-os.com/). However, my encryption method is not the exact method that Hacknet uses. Here is some differences between my encryption method and Hacknet encryption methoud:
- Hacknet uses a simple sum to encrypt every char; Mine uses the rewritten VBMath class to encrypt data.
- Hacknet encrypts every text character; Mine encrypts every byte.
- When Hacknet encrypts a file with password, it uses `GetHashCode` on password to get a seed; Mine uses a custom `GetSeed` method to get the seed.[See here](https://github.com/HirbodBehnam/Encrypher/blob/master/Encrypher/Encrypher.cpp#L596)
- Hacknet stores the password (HashCode) in the encrypted file; Mine does not.
