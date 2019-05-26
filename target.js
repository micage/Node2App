const spawn = require('child_process').spawn;

function spawnAsPromise() {
    var args = Array.prototype.slice.call(arguments);
    return new Promise(function (resolve, reject) {
        var stdout = '', stderr = '';

        var options = {
            cwd: '.',
            // env: process.env,
            // setsid: false,
            shell: true
        };
        var cp = spawn(args[0], args.slice(1), { shell: true }); // child process

        cp.stdout.on('data', function (chunk) {
            stdout += chunk;
        });
        cp.stderr.on('data', function (chunk) {
            stderr += chunk;
        });
        cp
            .on('error', reject)
            .on('close', function (code) {
                if (code === 0) {
                    resolve(stdout);
                } else {
                    reject(stderr);
                }
            });
    });
}

const Manager = function (args) {
    // TODO normalize args
    Object.assign(this, args);
    this.targets = {};
};
const Manager_methods = {
    addTarget(name, args) {
        this.targets[name] = new Target(args);
    },

    // sort out dependencies of targets
    work() {

    }
};
Object.assign(Manager.prototype, Manager_methods);


const Target = function (args) {
    // TODO normalize args
    Object.assign(this, args);
};
const Target_methods = {
    foo() { },

    /** @returns {Promise} */
    work() {
        // TODO make a string array out of our data
        let buildParams = [
            this.tool,
        ];
        return spawnAsPromise(...buildParams)
    }
};
Object.assign(Target.prototype, Target_methods);

module.exports = Target;