import os
import PyPDF2
import shutil
import datetime

# Log utility
def log_message(message, logfile="pdf_organizer.log"):
    with open(logfile, "a") as log:
        timestamp = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        log.write(f"[{timestamp}] {message}\n")

# Extract metadata from a PDF
def extract_metadata(pdf_path):
    try:
        with open(pdf_path, "rb") as f:
            reader = PyPDF2.PdfReader(f)
            info = reader.metadata
            title = info.title if info.title else "Unknown_Title"
            author = info.author if info.author else "Unknown_Author"
            return title.strip(), author.strip()
    except Exception as e:
        log_message(f"Failed to extract metadata from {pdf_path}: {str(e)}")
        return "Unknown_Title", "Unknown_Author"

# Sanitize file names
def sanitize_filename(name):
    return "".join(c for c in name if c.isalnum() or c in (" ", "_", "-")).rstrip()

# Rename file
def rename_pdf(original_path, new_name):
    directory = os.path.dirname(original_path)
    new_path = os.path.join(directory, new_name + ".pdf")
    if os.path.exists(new_path):
        base = new_name
        i = 1
        while os.path.exists(new_path):
            new_path = os.path.join(directory, f"{base}_{i}.pdf")
            i += 1
    os.rename(original_path, new_path)
    log_message(f"Renamed {original_path} to {new_path}")
    return new_path

# Move file to a folder
def move_to_folder(file_path, folder_name):
    folder_path = os.path.join(os.path.dirname(file_path), folder_name)
    if not os.path.exists(folder_path):
        os.makedirs(folder_path)
    new_path = os.path.join(folder_path, os.path.basename(file_path))
    shutil.move(file_path, new_path)
    log_message(f"Moved {file_path} to {new_path}")
    return new_path

# Organize all PDFs in a directory
def organize_pdfs(directory, rename=False, organize_by="author"):
    log_message(f"Starting organization in {directory}")
    for filename in os.listdir(directory):
        if filename.lower().endswith(".pdf"):
            full_path = os.path.join(directory, filename)
            title, author = extract_metadata(full_path)

            if rename:
                new_filename = sanitize_filename(f"{author}_{title}")
                full_path = rename_pdf(full_path, new_filename)

            folder = sanitize_filename(author) if organize_by == "author" else sanitize_filename(title)
            move_to_folder(full_path, folder)
    log_message("Finished organizing.")

# Simple CLI
def menu():
    print("=== PDF Organizer Tool ===")
    print("1. Organize PDFs by Author")
    print("2. Organize PDFs by Title")
    print("3. Rename and Organize by Author")
    print("4. Rename and Organize by Title")
    print("5. Exit")

    while True:
        choice = input("Choose an option (1-5): ")
        if choice in ['1', '2', '3', '4']:
            dir_path = input("Enter the path to your PDF folder: ").strip()
            if not os.path.isdir(dir_path):
                print("Invalid directory.")
                continue

            if choice == '1':
                organize_pdfs(dir_path, rename=False, organize_by="author")
            elif choice == '2':
                organize_pdfs(dir_path, rename=False, organize_by="title")
            elif choice == '3':
                organize_pdfs(dir_path, rename=True, organize_by="author")
            elif choice == '4':
                organize_pdfs(dir_path, rename=True, organize_by="title")

            print("Done. Check your folders and logs.")
        elif choice == '5':
            print("Exiting.")
            break
        else:
            print("Invalid option.")

if __name__ == "__main__":
    menu()

