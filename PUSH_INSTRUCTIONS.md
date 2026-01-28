# Push to GitHub Instructions

Your repository is now ready to be pushed to GitHub!

## Repository Information

- **Remote URL:** https://github.com/ManveerAnand/academic-site.git
- **Branch:** main
- **Status:** ✅ Ready to push

## What's Been Done

✅ **Repository Setup:**
- Initialized Git repository
- Added remote: `origin → https://github.com/ManveerAnand/academic-site.git`
- Created `.gitignore` to exclude unnecessary files

✅ **File Organization:**
- Created `cryptography/` folder structure
- Moved all assignments to `cryptography/assignments/`
- Moved all labs to `cryptography/labs/`
- Moved all exam tools to `cryptography/exam-prep/`
- Created resources folder with guides

✅ **Documentation Added:**
- Main README.md at root
- Comprehensive cryptography/README.md
- Individual READMEs for each subfolder
- CHEAT_SHEET.md for quick reference
- COMPILATION_GUIDE.md for building programs

✅ **Cleanup:**
- Removed executables (*.exe)
- Removed PDFs
- Removed zip files
- Removed old directory structures
- Removed build artifacts

✅ **Initial Commit:**
- All files staged and committed
- Commit message: "Initial commit: Add cryptography course materials"

## How to Push

### Option 1: Push to Main Branch (If repository is empty)
```bash
cd "d:\Dev 2.0\Crypto"
git push -u origin main
```

### Option 2: Push to New Branch (If repository has existing content)
```bash
cd "d:\Dev 2.0\Crypto"
git checkout -b cryptography-course
git push -u origin cryptography-course
```

Then create a Pull Request on GitHub to merge into main.

### Option 3: Force Push (Use with caution!)
If you want to replace everything in the repository:
```bash
cd "d:\Dev 2.0\Crypto"
git push -u origin main --force
```
⚠️ **Warning:** This will overwrite the remote repository!

## After Pushing

1. **Visit your repository:**
   https://github.com/ManveerAnand/academic-site

2. **Verify the structure:**
   - Check that the cryptography folder appears
   - Open README.md to see if it renders properly
   - Navigate through the folders

3. **Optional: Add repository description on GitHub**
   - "Academic coursework and projects collection"

4. **Optional: Add topics/tags:**
   - cryptography
   - c-programming
   - education
   - algorithms
   - security

## Repository Structure (After Push)

```
academic-site/
├── .gitignore
├── README.md                                    # Main repository overview
└── cryptography/                                # Cryptography course
    ├── README.md                                # Course overview
    ├── assignments/                             # All assignments
    │   ├── README.md
    │   ├── 202351080.c
    │   ├── assign1.c
    │   ├── ASSIGN2.c
    │   └── LAB_A3_202351080.c
    ├── labs/                                    # Lab exercises
    │   ├── README.md
    │   ├── aes_R1.c
    │   ├── fiestel.c
    │   ├── fiestel2.c
    │   ├── mixcol.cpp
    │   ├── square_n_mul.c
    │   └── ... (more labs)
    ├── exam-prep/                               # Exam tools
    │   ├── README.md
    │   ├── MENU_OPTIONS.txt
    │   ├── aes.c
    │   ├── rsa.c
    │   ├── number_theory.c
    │   ├── diffie_hellman.c
    │   └── ... (more tools)
    └── resources/                               # Reference materials
        ├── CHEAT_SHEET.md
        └── COMPILATION_GUIDE.md
```

## Troubleshooting

### Authentication Required
If GitHub asks for authentication:

**Using Personal Access Token (Recommended):**
1. Go to GitHub Settings → Developer Settings → Personal Access Tokens
2. Generate new token (classic)
3. Select scopes: `repo` (full control)
4. Copy the token
5. Use token as password when pushing

**Using GitHub CLI (Alternative):**
```bash
# Install GitHub CLI first
gh auth login
git push -u origin main
```

### Repository Already Has Content
If the remote repository isn't empty:

1. **Pull first:**
   ```bash
   git pull origin main --allow-unrelated-histories
   ```

2. **Resolve any conflicts**

3. **Push:**
   ```bash
   git push -u origin main
   ```

### Permission Denied
Make sure you have push access to the repository. If it's not your repository, you may need to fork it first.

## Next Steps (After Successful Push)

1. ✅ **Enable GitHub Pages (Optional):**
   - Go to Settings → Pages
   - Select branch: main
   - Your README will be accessible as a website!

2. ✅ **Add Collaborators (If needed):**
   - Settings → Collaborators
   - Invite team members

3. ✅ **Set up Branch Protection (Optional):**
   - Protect main branch from force pushes
   - Require pull request reviews

4. ✅ **Add More Courses:**
   - Follow same structure for other courses
   - Create new folders: `data-structures/`, `algorithms/`, etc.

## Quick Commands Reference

```bash
# Check status
git status

# View remote
git remote -v

# Push to GitHub
git push -u origin main

# Pull latest changes
git pull origin main

# Create new branch
git checkout -b new-branch-name

# See commit history
git log --oneline

# View changes
git diff
```

## Support

If you encounter any issues:
1. Check GitHub's authentication documentation
2. Ensure you have internet connection
3. Verify repository permissions
4. Check Git configuration

---

**You're all set! 🚀**

Run this command to push:
```bash
git push -u origin main
```

---

*Created: January 28, 2026*
