const quotes = [
    "The stars don’t look bigger, but they do look brighter. – Sally Ride",
    "Look at the stars. See their beauty. And in that beauty, see yourself. – Draya Mooney",
    "For my part I know nothing with any certainty, but the sight of the stars makes me dream. – Vincent Van Gogh",
];

document.getElementById("exploreButton").addEventListener("click", () => {
    const randomIndex = Math.floor(Math.random() * quotes.length);
    document.getElementById("quoteDisplay").textContent = quotes[randomIndex];
});
