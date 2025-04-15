const uploadForm = document.getElementById("uploadForm");
const fileList = document.getElementById("fileList");

const CGI_URL = "http://localhost:8082/cgi";

uploadForm.addEventListener("submit", async (e) => {
    e.preventDefault();
    const formData = new FormData(uploadForm);
    formData.append("action", "upload");

    const res = await fetch(CGI_URL, {
        method: "POST",
        body: formData,
    });
    const data = await res.json();
    if (data.status === "success") {
        loadFiles();
    } else {
        alert("Upload failed.");
    }
});

async function loadFiles() {
    const formData = new FormData();
    formData.append("action", "list");
    const res = await fetch(CGI_URL, {
        method: "POST",
        body: formData,
    });
    const files = await res.json();
    fileList.innerHTML = "";
    files.forEach((file) => {
        const li = document.createElement("li");
        li.innerHTML = `
      ${file}
      <button onclick="deleteFile('${file}')">Delete</button>
    `;
        fileList.appendChild(li);
    });
}

async function deleteFile(filename) {
    const formData = new FormData();
    formData.append("action", "delete");
    formData.append("filename", filename);
    const res = await fetch(CGI_URL, {
        method: "POST",
        body: formData,
    });
    const result = await res.json();
    if (result.status === "deleted") {
        loadFiles();
    } else {
        alert("Delete failed.");
    }
}

loadFiles();
