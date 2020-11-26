/*
    Javascipt file to handle taking data from file when dropped into the dropbox, and for the general working of the dropbox in the site
*/

// Give error messages on inappropriate file input or if browser doesnt support the function.
function reportError(msg) {
    document.getElementById("feedback").innerHTML=msg;
}

function clearError() {
    document.getElementById("feedback").innerHTML="";
}

function handleDragStart(e) {
    clearError();  // this / e.target is the source node.
}

function handleDragOver(e) {
    clearError()

    if (e.preventDefault) {
        e.preventDefault(); // Necessary. Allows us to drop.
    }

    e.dataTransfer.dropEffect = 'copy';  // See the section on the DataTransfer object.

    return false;
}

function handleDragEnter(e) {
    // this / e.target is the current hover target.
    this.classList.add('over');
}

function handleDragLeave(e) {
    this.classList.remove('over');  // this / e.target is previous target element.
}

function handleDragEnd(e) {
    //this.classList.remove('over');
    // this/e.target is the source node.
    var cols = document.querySelectorAll('.drop_zone');
    [].forEach.call(cols, function (col) {
        col.classList.remove('over');
    });
}


function getFile(e) {
    var files = e.dataTransfer.files;
    if(files.length <= 0){
        reportError("please drop a file")
        return null;
    }
    if(files.length > 1){
        reportError("please drop only one file")
        return null;
    }
    return files[0]
}

// Reads file and loads either the loadBenchmark or loadSolution functions from the other js files depending on which dropbox was used
function readFileAndLoad(file, loader) {
    if (!window.FileReader) {
        reportError('your browser does not have the necessary file reader API.');
        return;
    }
    
    var reader = new FileReader();

    reader.onload = function(e) {
        //document.getElementById('debug').innerHTML = e.target.result;
        loader(e.target.result)
    }

    reader.onerror = function(e) {
        console.log(e.target);
    }

    reader.readAsText(file);
}

// Handles event of file being dropped in benchmark dropbox
function handleDropBenchmark(e) {
    // this / e.target is current target element.
    e.stopPropagation(); // stops the browser from redirecting.
    e.preventDefault();
    
    var cols = document.querySelectorAll('.drop_zone');
    [].forEach.call(cols, function (col) {
        col.classList.remove('over');
    });
    
    // See the section on the DataTransfer object.
    file = getFile(e)
    if(file == null){
        return false;
    }

    //document.getElementById('debug').innerHTML = file; 
    readFileAndLoad(file, loadBenchmark);
    
    return false;
}

// Handles event of file being dropped in solutions dropbox
function handleDropSolution(e) {
    // this / e.target is current target element.
    e.stopPropagation(); // stops the browser from redirecting.
    e.preventDefault();

    var cols = document.querySelectorAll('.drop_zone');
    [].forEach.call(cols, function (col) {
        col.classList.remove('over');
    });
    
    // See the section on the DataTransfer object.
    file = getFile(e)
    if(file == null){
        return false;
    }

    readFileAndLoad(file, loadSolution);
    
    return false;
}

/*
  Setup Handelers
  Adds event handlers
 */
function setup() {

	var cols = document.querySelectorAll('.drop_zone');
	[].forEach.call(cols, function(col) {
		col.addEventListener('dragstart', handleDragStart, false);
		col.addEventListener('dragenter', handleDragEnter, false);
		col.addEventListener('dragleave', handleDragLeave, false);
		col.addEventListener('dragover',  handleDragOver, false);
		col.addEventListener('dragend',   handleDragEnd, false);
	});

	var cols = document.querySelectorAll('#benchmark_zone');
	[].forEach.call(cols, function(col) {
		col.addEventListener('drop', handleDropBenchmark, false);
	});

	var cols = document.querySelectorAll('#solution_zone');
	[].forEach.call(cols, function(col) {
		col.addEventListener('drop', handleDropSolution, false);
	});

}

setup();



