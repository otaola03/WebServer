// Select the image file input element
const imageInput = document.querySelector('#image-input');

// Add an event listener to the form submit button
document.querySelector('#submit-button').addEventListener('click', async (event) => {
	event.preventDefault();

	// Get the selected image file
	const imageFile = imageInput.files[0];

	// Create a new FormData object
	const formData = new FormData();

	// Append the image file to the FormData object
	formData.append('image', imageFile);

	try {
		// Send a POST request to localhost:8000 with the FormData object as the body
		const response = await fetch('http://localhost:8000', {
			method: 'POST',
			body: formData
		});

		// Log the response
		console.log(response);
	} catch (error) {
		// Log any errors
		console.error(error);
	}
});
