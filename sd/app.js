const root = document.getElementById("root");
const template = document.getElementById("template").content.firstElementChild;
const upButton = document.getElementById("up-button");

function getCurrentPath()
{
    const currentPath = window.location.pathname;

    // replace currentPath with '' if it's '/' (to avoid double slash)
    return currentPath == '/' ? '' : currentPath;
}

function createItem({name, size, isDirectory, peek, index})
{
    // convert strings to correct types
    size = parseInt(size);
    isDirectory = isDirectory == "true";
    peek = parseInt(peek);
    index = parseInt(index);


    const item = template.cloneNode(true);

    const nameA = item.querySelector(".name > button");
    nameA.textContent = name;
    nameA.addEventListener("click", () => {
        if (isDirectory) { // directory pushes state to fake page transition
            window.history.pushState(null, null,
                `${window.location.origin}${getCurrentPath()}/${name}`
            );

            doPageUpdate();

        } else {
            // just open the file
            window.location.href = `/api/file${getCurrentPath()}/${name}`;
        }
    });

    item.querySelector(".size").textContent = isDirectory ? '' : `${size} bytes`;

    item.querySelector(".peek-binary").textContent = (peek >>> 0).toString(2); // https://stackoverflow.com/a/16155417
    item.querySelector(".peek").textContent = `${String.fromCharCode(peek)} ; ${peek}`;

    item.querySelector(".index").textContent = index;

    root.appendChild(item);
}


let pageData = [];

async function loadPageData()
{
    const url = window.location.pathname;

    return await
        fetch(`/api/folder${url}`)
        .then(response => response.json())
        .then(data => pageData = data);
}
function loadInPage()
{
    // remove old items
    root.textContent = '';

    // add new items
    pageData.forEach(createItem);
}
function doPageUpdate()
{
    // fetch page data
    loadPageData()
        .then( // then display it
            () => loadInPage()
        );
}

function goUpAFolder()
{
    let path = window.location.pathname; // get url

    if (path == '/') { // if root, make button flash red
        upButton.classList.add("root");
        setTimeout(() => upButton.classList.remove("root"), 1);

        return; // can't go up from root
    }

    path = path.substring(0, path.lastIndexOf('/')); // get everything before last slash

    window.history.pushState(null, null, `${window.location.origin}${path}`); // push state to fake page transition

    doPageUpdate();
}
upButton.addEventListener("click", goUpAFolder);

// update page data on page back pressed
window.addEventListener("popstate", doPageUpdate);

// startup
doPageUpdate();
