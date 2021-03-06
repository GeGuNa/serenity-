test("basic functionality", () => {
    let d = new Date(2000, 2, 1);

    d.setSeconds(50);
    expect(d.getSeconds()).toBe(50);

    d.setSeconds(50, 600);
    expect(d.getSeconds()).toBe(50);
    expect(d.getMilliseconds()).toBe(600);
});
