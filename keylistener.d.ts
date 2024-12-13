declare module "keylistener" {
    /**
     * Starts the key listener.
     * @param callback - A function that will be called with the key code of each key press.
     */
    export function start(callback: (keyCode: number) => void): void;

    /**
     * Stops the key listener.
     */
    export function stop(): void;
}
