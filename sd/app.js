const template = document.getElementById("template").content.firstElementChild;
const root = document.getElementById("root");

root.textContent = '';

function createItem({name, size, isDirectory, peek, index})
{
    // convert strings to correct types
    size = parseInt(size);
    isDirectory = isDirectory == "true";
    index = parseInt(index);


    const item = template.cloneNode(true);

    const nameA = item.querySelector(".name > button");
    nameA.textContent = name;
    nameA.addEventListener("click", () => {
        if (isDirectory) {
            console.log(`directory ${name} clicked`);

        } else {
            console.log(`file ${name} clicked`);
        }
    });

    if (!isDirectory)
        item.querySelector(".size").textContent = `${size} bytes`;

    else
        item.querySelector(".size").hidden = true;

    item.querySelector(".peek-binary").textContent = (parseInt(peek) >>> 0).toString(2); // https://stackoverflow.com/a/16155417
    item.querySelector(".peek").textContent = `${String.fromCharCode(parseInt(peek))} ; ${peek}`;

    item.querySelector(".index").textContent = index;

    root.appendChild(item);
}

for (let i = 0; i < 10; i++)
{
    createItem({
        name: `SUPER-~${i}.TXT`,
        size: i * 1000,
        isDirectory: "false",
        peek: 66,
        index: i
    });
}
