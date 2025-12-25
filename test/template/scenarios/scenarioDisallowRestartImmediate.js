var CodePushWrapper = require("../codePushWrapper.js");
import CodePush from "@d11/codepush";

module.exports = {
    startTest: function (testApp) {
        CodePush.disallowRestart();
        CodePushWrapper.checkAndInstall(testApp,
            () => {
                CodePush.allowRestart();
            },
            undefined,
            CodePush.InstallMode.IMMEDIATE,
            undefined,
            true
        );
    },

    getScenarioName: function () {
        return "disallowRestart";
    }
};
