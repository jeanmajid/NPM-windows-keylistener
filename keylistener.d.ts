declare module "@jeanmajid/windows-keylistener" {
    /**
     * Starts the key listener.
     * @param callback - A function that will be called with the key code of each key press.
     */
    export function start(callback: (keyCode: number) => void): void;

    /**
     * Stops the key listener.
     */
    export function stop(): void;

    /**
     * Copy a text to the clipboard
     * @param text The text to copy to the clipboard
     */
    export function copyToClipboard(text: string): void;
}
