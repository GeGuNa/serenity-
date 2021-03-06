test("basic functionality", () => {
    let d = new Date(2000, 2, 1);

    d.setMinutes(30);
    expect(d.getMinutes()).toBe(30);

    d.setMinutes(30, 50);
    expect(d.getMinutes()).toBe(30);
    expect(d.getSeconds()).toBe(50);

    d.setMinutes(30, 50, 600);
    expect(d.getMinutes()).toBe(30);
    expect(d.getSeconds()).toBe(50);
    expect(d.getMilliseconds()).toBe(600);
});
