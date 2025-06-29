declare module "@jeanmajid/windows-keylistener" {
    /**
     * Starts the key listener.
     * @param callback - A function that will be called with the key code and state of each key event.
     */
    export function start(callback: (keyCode: number, isPressed: boolean) => void): void;

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