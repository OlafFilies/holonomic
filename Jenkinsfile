pipeline {
	agent {
		node {
			label 'rpi'
			customWorkspace '/home/jenkins/iot/holonomic'
		}
	}

	options {
		buildDiscarder(
			logRotator(
				numToKeepStr: '5',
				artifactNumToKeepStr: '5'
			)
		)
		disableConcurrentBuilds()
	}

	/*
	triggers {
		cron('H H(0-1) * * *')
	}
	*/
	stages {
		stage("SCM") {
			steps {
				checkout( [
					$class: 'GitSCM', 
					branches: [
						[ name: '*' ]
					], 
					doGenerateSubmoduleConfigurations: false, 
					extensions: [
						[
							$class: 'SubmoduleOption', 
							disableSubmodules: false, 
							parentCredentials: true, 
							recursiveSubmodules: true, 
							reference: '${WORKSPACE}/qml',
							trackingSubmodules: false
						]
					],
 					submoduleCfg: [],
					userRemoteConfigs: [
						[
							credentialsId: '92776ba5-4415-4b42-9271-d39b1ab88ef6',
							url: 'ssh://marcus@situla.fritz.box:29418/~marcus/robot.git'
						]
					]
				] )
			}
		}

		stage('Build documentation') {
			parallel {
				/*-----------------------------------------------------------------------------
				 * html
				 *-----------------------------------------------------------------------------
				 */
				stage('HTML') {
					steps {
						echo "html"
						build(
							job: 'dev/doxy_maker',
							propagate: true,
							parameters: [
								[
									$class: 'StringParameterValue',
									name: 'WORKSPACE',
									value: "${env.WORKSPACE}"
								],
							]
						)
					}
				}
				/*-----------------------------------------------------------------------------
				 * latex/pdf
				 *-----------------------------------------------------------------------------
				 */
				stage('PDF') {
					steps {
						echo "latex/pdf"
						/*
						build(
							job: 'dev/doxy_maker_pdf',
							propagate: true,
							parameters: [
								[
									$class: 'StringParameterValue',
									name: 'WORKSPACE',
									value: "${env.WORKSPACE}"
								],
							]
						)
						*/
					}
				}
			} // end parallel
		} // end docu build

		stage('Deploy documentation') {
			parallel {
				/*-----------------------------------------------------------------------------
				 * html
				 *-----------------------------------------------------------------------------
				 */
				stage('HTML deploy') {
					steps {
						echo "html deploy"
/*						build(
							job: 'dev/IoT/libraries/IoT deploy HTML Documentation',
							propagate: true,
							parameters: [
								[
									$class: 'StringParameterValue',
									name: 'DOCPATH',
									value: 'Lego_Robot/libraries/Tle94112/docs/html/'
								],
								[
									$class: 'StringParameterValue',
									name: 'DEPLOYPATH',
									value: 'iot/Tle94112/'
								]
							]
						)
*/					}
				}
			} // end parallel
		} // end deploy docu

		stage('Build') {
			parallel {
				/*-----------------------------------------------------------------------------
				 * development
				 *-----------------------------------------------------------------------------
				 */
				stage('DEVELOPMENT') {
    				environment {
        				scannerHome = tool 'SonarQubeScanner'
    				}
					steps {
						echo "Development"
				        withSonarQubeEnv('SonarQube') {
            				sh "${scannerHome}/bin/sonar-scanner -X"
        				}
						timeout(time: 10, unit: 'MINUTES') {
            				waitForQualityGate abortPipeline: true
        				}
					}
				}

				/*-----------------------------------------------------------------------------
				 * staging
				 *-----------------------------------------------------------------------------
				 */
				stage('STAGING') {
					steps {
						echo "Staging"
						/*
						build(
							job: 'dev/I-Tec/CI-CD/task I-Tec grantDB',
							propagate: true,
							parameters: [
								[
									$class: 'StringParameterValue',
									name: 'RAZWARE_TIER',
									value: 'staging'
								],
								[
									$class: 'StringParameterValue',
									name: 'RAZWEB_ROOT',
									value: '/home/razweb/htdocs-vihlarazweb4'
								],
								[
									$class: 'StringParameterValue',
									name: 'RAMEZ_ROOT_URL',
									value: 'https://vihlarazweb4.vih.infineon.com'
								]
							]
						)
						*/
					}
				}

				/*-----------------------------------------------------------------------------
				 * production
				 *-----------------------------------------------------------------------------
				 */
				stage('PRODUCTION') {
					steps {
						echo "Production"
						/*
						build(
							job: 'dev/I-Tec/CI-CD/task I-Tec grantDB',
							propagate: true,
							parameters: [
								[
									$class: 'StringParameterValue',
									name: 'RAZWARE_TIER',
									value: 'production'
								],
								[
									$class: 'StringParameterValue',
									name: 'RAZWEB_ROOT',
									value: '/home/razweb/htdocs-vihlarazweb1'
								],
								[
									$class: 'StringParameterValue',
									name: 'RAMEZ_ROOT_URL',
									value: 'https://vihlarazweb1.vih.infineon.com'
								]
							]
						)
						*/
					}
				}

			} // end parallel
		} // end stage
	} // end stages

	/*-----------------------------------------------------------------------------
	 * post
	 *-----------------------------------------------------------------------------
	 */
	post {
		failure {
			mail(
				to: 'olaf.filies@t-online.de',
				subject: "[EYENET JENKINS] holonomic pipeline:",
				body: "Something during Jenkins pipeline run went wrong."
			)
		}
	}

}
